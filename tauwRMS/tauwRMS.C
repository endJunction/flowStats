/**
 * \file
 * \author Dmitri Naumov (dmitri.naumov@ufz.de)
 *
 * \copyright 2012 -- Helmholtz Centre for Environmental Research - UFZ
 * distributed under the GNU GPL version 3. See the LICENSE file for details.
 *
 * \brief Root-mean-square of a vector field.
 *
 * Calculates the root-mean-sqare of a vector field, which name is given on
 * command line.
 */

#include "fvCFD.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
    argList::validArgs.append("patchName");

    #include "setRootCase.H"
    #include "createTime.H"

    instantList timeDirs = timeSelector::select0(runTime, args);

    const word vecFieldName = "wallShearStress";

    #include "createMesh.H"

    runTime.setTime(timeDirs.last(), timeDirs.size()-1);

    const word patchName = args[1];

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        Info << "    Read vector field " << vecFieldName << endl;

        IOobject vecFieldHeader
        (
            vecFieldName,
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!vecFieldHeader.headerOk())
        {
            Info << "Unable to read vector field" << vecFieldName << endl;
            return EXIT_FAILURE;
        }

        const label patchI = mesh.boundaryMesh().findPatchID(patchName);
        if (patchI < 0)
        {
            FatalError << "Unable to find patch " << patchName << nl
                << exit(FatalError);
        }

        const volVectorField vectorField(vecFieldHeader, mesh);
        const scalarField magVecField = magSqr(vectorField.boundaryField()[patchI]);

        const scalar area = gSum(mesh.magSf().boundaryField()[patchI]);
        scalar meanSqr = gSum(magVecField * mesh.magSf().boundaryField()[patchI]) / area;

        Info << "    RMS(mag(" << vecFieldName << ")) = " << Foam::sqrt(meanSqr) << endl;
        Info << "    max(mag(" << vecFieldName << ")) = " << Foam::sqrt(Foam::gMax(magVecField));
        Info << endl;
    }

    return 0;
}


// ************************************************************************* //
